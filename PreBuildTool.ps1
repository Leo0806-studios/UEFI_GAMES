# yeah yeah. i know this is ai generated. im lazy. sue me
[CmdletBinding()]
param (
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$CompilerPath,

    [Parameter(Mandatory = $true, Position = 1)]
    [string]$DestinationPath,

    [Parameter(Mandatory = $false, Position = 2)]
    [string[]]$FileNames = @(
        "yvals.h",
        "vcruntime.h",
        "vcruntime_string.h"
    ),

    # Internal parameters used by the elevated instance.
    [Parameter(Mandatory = $false)]
    [switch]$Elevated,

    [Parameter(Mandatory = $false)]
    [string]$LogFile
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"


# ============================================================================
# Logging and summary helpers
# ============================================================================

$Script:RunLogFile = $null
$Script:FileIssues = New-Object System.Collections.Generic.List[object]

function Write-Log {
    param (
        [string]$Message
    )

    Write-Host $Message

    if ($Script:RunLogFile) {
        Add-Content -LiteralPath $Script:RunLogFile -Value $Message
    }
}

function Initialize-RunLogFile {
    param (
        [Parameter(Mandatory = $true)]
        [string]$Directory
    )

    $Timestamp = Get-Date -Format 'yyyyMMdd-HHmmssfff'
    $Script:RunLogFile = Join-Path $Directory "PreBuildTool-$Timestamp.log"

    New-Item -ItemType File -Path $Script:RunLogFile -Force | Out-Null

    Write-Log "Log file         : $Script:RunLogFile"
}

function Add-FileIssue {
    param (
        [Parameter(Mandatory = $true)]
        [string]$FileName,

        [Parameter(Mandatory = $true)]
        [string]$Issue,

        [string]$Detail
    )

    $Script:FileIssues.Add([PSCustomObject]@{
        FileName = $FileName
        Issue    = $Issue
        Detail   = $Detail
    })
}

function Write-FinalReport {
    param (
        [Parameter(Mandatory = $true)]
        [string]$LogDirectory
    )

    Write-Log ""
    Write-Log "============================================================"
    Write-Log "RUN SUMMARY"

    if ($Script:FileIssues.Count -eq 0) {
        Write-Log "No file issues detected."
    }
    else {
        Write-Log "Files with issues:"

        foreach ($IssueRecord in $Script:FileIssues) {
            Write-Log "    $($IssueRecord.FileName)"
            Write-Log "        Issue : $($IssueRecord.Issue)"

            if ($IssueRecord.Detail) {
                Write-Log "        Detail : $($IssueRecord.Detail)"
            }
        }
    }

    Write-Log "============================================================"

    if ($Script:RunLogFile) {
        $LogFiles = Get-ChildItem -LiteralPath $LogDirectory -Filter 'PreBuildTool-*.log' -File | Sort-Object LastWriteTime -Descending

        $LogFilesToRemove = $LogFiles | Select-Object -Skip 5

        foreach ($OldLog in $LogFilesToRemove) {
            if ($OldLog.FullName -ne $Script:RunLogFile) {
                Remove-Item -LiteralPath $OldLog.FullName -Force -ErrorAction SilentlyContinue
            }
        }
    }
}


# ============================================================================
# Check administrator status
# ============================================================================

$CurrentIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()

$Principal = New-Object Security.Principal.WindowsPrincipal($CurrentIdentity)

$IsAdministrator = $Principal.IsInRole(
    [Security.Principal.WindowsBuiltInRole]::Administrator
)


# ============================================================================
# NORMAL INSTANCE
#
# If we're not elevated, start an elevated copy of this script.
# ============================================================================

Write-Host "============================================================"
Write-Host "PreBuildTool starting"
Write-Host "PID             : $PID"
Write-Host "Compiler        : $CompilerPath"
Write-Host "Destination     : $DestinationPath"
Write-Host "Elevated        : $IsAdministrator"
Write-Host "============================================================"

if ($IsAdministrator) {
    Write-Host "Already running as Administrator."
}
else {
    Write-Host "Running without elevation."
}

if ($Elevated -and $LogFile) {
    Write-Log "Elevated instance received log file: $LogFile"
}



# ============================================================================
# ELEVATED INSTANCE
# ============================================================================

if ($Elevated) {

    Write-Log ""
    Write-Log "============================================================"
    Write-Log "ELEVATED INSTANCE STARTED"
    Write-Log "PID             : $PID"
    Write-Log "IsAdministrator  : $IsAdministrator"
    Write-Log "Compiler        : $CompilerPath"
    Write-Log "Destination     : $DestinationPath"
    Write-Log "LogFile         : $LogFile"
    Write-Log "============================================================"
}


# ============================================================================
# Win32 API for identifying hard-linked files
# ============================================================================

Add-Type @'
using System;
using System.Runtime.InteropServices;

namespace HardLinkTools
{
    public static class NativeMethods
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct BY_HANDLE_FILE_INFORMATION
        {
            public uint FileAttributes;
            public System.Runtime.InteropServices.ComTypes.FILETIME CreationTime;
            public System.Runtime.InteropServices.ComTypes.FILETIME LastAccessTime;
            public System.Runtime.InteropServices.ComTypes.FILETIME LastWriteTime;
            public uint VolumeSerialNumber;
            public uint FileSizeHigh;
            public uint FileSizeLow;
            public uint NumberOfLinks;
            public uint FileIndexHigh;
            public uint FileIndexLow;
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern IntPtr CreateFile(
            string lpFileName,
            uint dwDesiredAccess,
            uint dwShareMode,
            IntPtr lpSecurityAttributes,
            uint dwCreationDisposition,
            uint dwFlagsAndAttributes,
            IntPtr hTemplateFile
        );

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool GetFileInformationByHandle(
            IntPtr hFile,
            out BY_HANDLE_FILE_INFORMATION lpFileInformation
        );

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool CloseHandle(
            IntPtr hObject
        );

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool CreateHardLink(
            string lpFileName,
            string lpExistingFileName,
            IntPtr lpSecurityAttributes
        );

        public const uint GENERIC_READ = 0x80000000;

        public const uint FILE_SHARE_READ  = 0x00000001;
        public const uint FILE_SHARE_WRITE = 0x00000002;
        public const uint FILE_SHARE_DELETE = 0x00000004;

        public const uint OPEN_EXISTING = 3;

        public static readonly IntPtr INVALID_HANDLE_VALUE =
            new IntPtr(-1);
    }
}
'@


# ============================================================================
# Get file identity
# ============================================================================

function Get-FileIdentity {
    param (
        [Parameter(Mandatory = $true)]
        [string]$Path
    )

    Write-Log "Getting file identity:"
    Write-Log "    $Path"

    $Handle = [HardLinkTools.NativeMethods]::CreateFile(
        $Path,
        [HardLinkTools.NativeMethods]::GENERIC_READ,
        [HardLinkTools.NativeMethods]::FILE_SHARE_READ `
            -bor [HardLinkTools.NativeMethods]::FILE_SHARE_WRITE `
            -bor [HardLinkTools.NativeMethods]::FILE_SHARE_DELETE,
        [IntPtr]::Zero,
        [HardLinkTools.NativeMethods]::OPEN_EXISTING,
        0,
        [IntPtr]::Zero
    )

    if ($Handle -eq [HardLinkTools.NativeMethods]::INVALID_HANDLE_VALUE) {

        $ErrorCode = [Runtime.InteropServices.Marshal]::GetLastWin32Error()

        throw "CreateFile failed for '$Path'. Win32 error: $ErrorCode"
    }

    try {

        $Info = New-Object `
            HardLinkTools.NativeMethods+BY_HANDLE_FILE_INFORMATION

        $Success = [HardLinkTools.NativeMethods]::GetFileInformationByHandle(
            $Handle,
            [ref]$Info
        )

        if (-not $Success) {

            $ErrorCode = [Runtime.InteropServices.Marshal]::GetLastWin32Error()

            throw "GetFileInformationByHandle failed for '$Path'. Win32 error: $ErrorCode"
        }

        return @{
            VolumeSerialNumber = $Info.VolumeSerialNumber
            FileIndexHigh      = $Info.FileIndexHigh
            FileIndexLow       = $Info.FileIndexLow
            NumberOfLinks      = $Info.NumberOfLinks
        }
    }
    finally {

        [HardLinkTools.NativeMethods]::CloseHandle($Handle) | Out-Null
    }
}


# ============================================================================
# Compare two files
# ============================================================================

function Test-SameFile {
    param (
        [string]$Path1,
        [string]$Path2
    )

    $File1 = Get-FileIdentity $Path1
    $File2 = Get-FileIdentity $Path2

    return (
        $File1.VolumeSerialNumber -eq $File2.VolumeSerialNumber -and
        $File1.FileIndexHigh -eq $File2.FileIndexHigh -and
        $File1.FileIndexLow -eq $File2.FileIndexLow
    )
}

function New-HardLink {
    param (
        [Parameter(Mandatory = $true)]
        [string]$Path,

        [Parameter(Mandatory = $true)]
        [string]$Target
    )

    $Success = [HardLinkTools.NativeMethods]::CreateHardLink(
        $Path,
        $Target,
        [IntPtr]::Zero
    )

    if (-not $Success) {
        $ErrorCode = [Runtime.InteropServices.Marshal]::GetLastWin32Error()
        throw "CreateHardLink failed for '$Path' -> '$Target'. Win32 error: $ErrorCode"
    }
}

function Set-FileFromSource {
    param (
        [Parameter(Mandatory = $true)]
        [string]$Path,

        [Parameter(Mandatory = $true)]
        [string]$Target,

        [string]$FileName
    )

    try {
        New-HardLink -Path $Path -Target $Target
        Write-Log "HARD LINK CREATED."
        return "Linked"
    }
    catch {
        Write-Log "Hard link creation failed; copying file instead."
        Write-Log $_.Exception.Message
    }

    try {
        Copy-Item -LiteralPath $Target -Destination $Path -Force -ErrorAction Stop
        Write-Log "FILE COPIED."
        return "Copied"
    }
    catch {
        if ($FileName) {
            Add-FileIssue `
                -FileName $FileName `
                -Issue "Failed to copy file" `
                -Detail $_.Exception.Message
        }

        throw
    }
}

# ============================================================================
# MAIN WORK
# ============================================================================

try {

    Write-Log ""
    Write-Log "Starting MSVC include processing..."


    # ------------------------------------------------------------------------
    # Validate compiler
    # ------------------------------------------------------------------------

    $CompilerPath = [System.IO.Path]::GetFullPath($CompilerPath)

    Write-Log "Compiler:"
    Write-Log "    $CompilerPath"

    if (-not (Test-Path -LiteralPath $CompilerPath -PathType Leaf)) {
        throw "Compiler does not exist: $CompilerPath"
    }


    # ------------------------------------------------------------------------
    # Find toolchain root
    # ------------------------------------------------------------------------

    $Current = Get-Item -LiteralPath $CompilerPath -Force

    $Current = $Current.Directory

    Write-Log "Starting directory search at:"
    Write-Log "    $($Current.FullName)"

    $ToolchainRoot = $null

    while ($null -ne $Current) {

        Write-Log "Checking:"
        Write-Log "    $($Current.FullName)"

        if ($Current.Parent -and $Current.Parent.Name -eq "MSVC") {

            $ToolchainRoot = $Current.FullName

            break
        }

        $Current = $Current.Parent
    }

    if (-not $ToolchainRoot) {
        throw "Could not locate MSVC toolchain root."
    }

    Write-Log "Toolchain root:"
    Write-Log "    $ToolchainRoot"


    # ------------------------------------------------------------------------
    # Include directory
    # ------------------------------------------------------------------------

    $IncludePath = Join-Path $ToolchainRoot "include"

    Write-Log "Include directory:"
    Write-Log "    $IncludePath"

    if (-not (Test-Path -LiteralPath $IncludePath -PathType Container)) {
        throw "Include directory does not exist: $IncludePath"
    }


    # ------------------------------------------------------------------------
    # Destination
    # ------------------------------------------------------------------------

    Write-Log "Destination:"
    Write-Log "    $DestinationPath"

    if (-not (Test-Path -LiteralPath $DestinationPath)) {

        Write-Log "Destination does not exist."
        Write-Log "Creating destination directory..."

        New-Item `
            -ItemType Directory `
            -Path $DestinationPath `
            -Force | Out-Null

        Write-Log "Destination created."
    }
    elseif (-not (Test-Path -LiteralPath $DestinationPath -PathType Container)) {

        throw "Destination exists but is not a directory: $DestinationPath"
    }

    Initialize-RunLogFile -Directory $DestinationPath


    # ------------------------------------------------------------------------
    # Process files
    # ------------------------------------------------------------------------

    Write-Log ""
    Write-Log "Files to process:"
    
    foreach ($FileName in $FileNames) {
        Write-Log "    $FileName"
    }

    Write-Log ""


    foreach ($FileName in $FileNames) {

        $SourcePath = Join-Path $IncludePath $FileName
        $LinkPath   = Join-Path $DestinationPath $FileName

        Write-Log "------------------------------------------------------------"
        Write-Log "Processing: $FileName"
        Write-Log "Source:"
        Write-Log "    $SourcePath"
        Write-Log "Destination:"
        Write-Log "    $LinkPath"


        # --------------------------------------------------------------------
        # Source validation
        # --------------------------------------------------------------------

        if (-not (Test-Path -LiteralPath $SourcePath -PathType Leaf)) {

            throw "SOURCE DOES NOT EXIST: $SourcePath"
        }


        # --------------------------------------------------------------------
        # Nothing exists at destination.
        # --------------------------------------------------------------------

        if (-not (Test-Path -LiteralPath $LinkPath)) {

            Write-Log "Destination does not exist."
            Write-Log "Creating file from source..."

            $ProvisionResult = Set-FileFromSource `
                -Path $LinkPath `
                -Target $SourcePath `
                -FileName $FileName

            if ($ProvisionResult -eq "Copied") {
                Add-FileIssue `
                    -FileName $FileName `
                    -Issue "Hard link could not be created" `
                    -Detail "Copied file instead."
            }

            continue
        }


        # --------------------------------------------------------------------
        # Something exists.
        # --------------------------------------------------------------------

        $ExistingItem = Get-Item -LiteralPath $LinkPath -Force

        Write-Log "Destination already exists."
        Write-Log "Existing type:"
        Write-Log "    $($ExistingItem.GetType().FullName)"

        if ($ExistingItem.LinkType) {
            Write-Log "Link type:"
            Write-Log "    $($ExistingItem.LinkType)"
        }


        # --------------------------------------------------------------------
        # Existing symbolic link.
        # --------------------------------------------------------------------

        if ($ExistingItem.LinkType -eq "SymbolicLink") {

            Write-Log "Existing object is a SYMBOLIC LINK."
            Write-Log "Removing it..."

            Remove-Item `
                -LiteralPath $LinkPath `
                -Force `
                -ErrorAction Stop

            Write-Log "Creating file from source..."

            $ProvisionResult = Set-FileFromSource `
                -Path $LinkPath `
                -Target $SourcePath `
                -FileName $FileName

            if ($ProvisionResult -eq "Copied") {
                Add-FileIssue `
                    -FileName $FileName `
                    -Issue "Hard link could not be created" `
                    -Detail "Copied file instead after replacing symbolic link."
            }

            continue
        }


        # --------------------------------------------------------------------
        # Existing regular file.
        # --------------------------------------------------------------------

        if (-not $ExistingItem.PSIsContainer) {

            Write-Log "Existing object is a regular file."

            Write-Log "Checking whether it is already the same file..."

            if (Test-SameFile $SourcePath $LinkPath) {

                Write-Log "YES."
                Write-Log "Existing file is already linked to the source."

                continue
            }

            Write-Log "Existing file is different. Replacing it..."

            Remove-Item `
                -LiteralPath $LinkPath `
                -Force `
                -ErrorAction Stop

            Write-Log "Creating file from source..."

            $ProvisionResult = Set-FileFromSource `
                -Path $LinkPath `
                -Target $SourcePath `
                -FileName $FileName

            if ($ProvisionResult -eq "Copied") {
                Add-FileIssue `
                    -FileName $FileName `
                    -Issue "Hard link could not be created" `
                    -Detail "Copied file instead after replacing an existing file."
            }

            continue
        }


        # --------------------------------------------------------------------
        # Existing directory.
        # --------------------------------------------------------------------

        throw "REFUSING TO OVERWRITE EXISTING DIRECTORY: $LinkPath"
    }


    # ------------------------------------------------------------------------
    # Done
    # ------------------------------------------------------------------------

    Write-Log ""
    Write-Log "============================================================"
    Write-Log "SUCCESS"
    Write-Log "All requested MSVC links have been processed."
    Write-Log "============================================================"

    Write-FinalReport -LogDirectory $DestinationPath

    exit 0
}
catch {

    Write-Log ""
    Write-Log "============================================================"
    Write-Log "ERROR"
    Write-Log $_.Exception.Message
    Write-Log "============================================================"

    Write-FinalReport -LogDirectory $DestinationPath

    exit 1
}
