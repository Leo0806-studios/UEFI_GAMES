# yeah yeah. i know this is ai generated. im lazy. sue me

[CmdletBinding()]
param (
    [Parameter(Mandatory = $true)]
    [string]$CompilerPath,

    [Parameter(Mandatory = $true)]
    [string]$UcrtPath,

    [Parameter(Mandatory = $true)]
    [string]$DestinationPath,

    [Parameter(Mandatory = $false)]
    [string[]]$FileNames = @(
        "yvals.h",
        "vcruntime.h",
        "vcruntime_string.h",
        "sal.h",
        "intrin.h",
        "concurrencysal.h",
        "ehdata_forceinclude.h",
        "vadefs.h",
        "intrin0.inl.h",
        "setjmp.h",
        "immintrin.h",
        "wmmintrin.h",
        "nmmintrin.h",
        "smmintrin.h",
        "tmmintrin.h",
        "pmmintrin.h",
        "emmintrin.h",
        "xmmintrin.h",
        "mmintrin.h",
        "malloc.h",
        "corecrt.h",
        "corecrt_malloc.h",
        "zmmintrin.h",
        "ammintrin.h",
        "stdint.h",
        "ehdata4_export.h"
    ),

    # Internal parameters retained for compatibility with any elevated
    # invocation mechanism.
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
$Script:LogBuffer = New-Object System.Collections.Generic.List[string]
$Script:FileIssues = New-Object System.Collections.Generic.List[object]


function Write-Log {
    param (
        [string]$Message
    )

    $Script:LogBuffer.Add($Message)
}


function Initialize-RunLogFile {
    param (
        [Parameter(Mandatory = $true)]
        [string]$Directory
    )

    $Timestamp = Get-Date -Format 'yyyyMMdd-HHmmssfff'
    $Script:RunLogFile = Join-Path $Directory "PreBuildTool-$Timestamp.log"

    Write-Log "Log file         : $Script:RunLogFile"
}


function Flush-RunLogFile {

    if (-not $Script:RunLogFile -or $Script:LogBuffer.Count -eq 0) {
        return
    }

    Set-Content `
        -LiteralPath $Script:RunLogFile `
        -Value $Script:LogBuffer
}


function Add-FileIssue {
    param (
        [Parameter(Mandatory = $true)]
        [string]$FileName,

        [Parameter(Mandatory = $true)]
        [string]$Issue,

        [string]$Detail
    )

    $Script:FileIssues.Add(
        [PSCustomObject]@{
            FileName = $FileName
            Issue    = $Issue
            Detail   = $Detail
        }
    )
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
            Write-Log "        Issue  : $($IssueRecord.Issue)"

            if ($IssueRecord.Detail) {
                Write-Log "        Detail : $($IssueRecord.Detail)"
            }
        }
    }

    Write-Log "============================================================"

    Flush-RunLogFile

    if ($Script:RunLogFile) {

        $LogFiles = Get-ChildItem `
            -LiteralPath $LogDirectory `
            -Filter 'PreBuildTool-*.log' `
            -File |
            Sort-Object LastWriteTime -Descending

        $LogFilesToRemove = $LogFiles | Select-Object -Skip 5

        foreach ($OldLog in $LogFilesToRemove) {

            if ($OldLog.FullName -ne $Script:RunLogFile) {

                Remove-Item `
                    -LiteralPath $OldLog.FullName `
                    -Force `
                    -ErrorAction SilentlyContinue
            }
        }
    }
}


# ============================================================================
# Check administrator status
# ============================================================================

$CurrentIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()

$Principal = New-Object Security.Principal.WindowsPrincipal(
    $CurrentIdentity
)

$IsAdministrator = $Principal.IsInRole(
    [Security.Principal.WindowsBuiltInRole]::Administrator
)


Write-Log "============================================================"
Write-Log "PreBuildTool starting"
Write-Log "PID             : $PID"
Write-Log "Compiler        : $CompilerPath"
Write-Log "UCRT            : $UcrtPath"
Write-Log "Destination     : $DestinationPath"
Write-Log "Administrator   : $IsAdministrator"
Write-Log "Elevated        : $Elevated"
Write-Log "============================================================"


# ============================================================================
# Win32 API for hard-link support
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
        (
            [HardLinkTools.NativeMethods]::FILE_SHARE_READ `
                -bor [HardLinkTools.NativeMethods]::FILE_SHARE_WRITE `
                -bor [HardLinkTools.NativeMethods]::FILE_SHARE_DELETE
        ),
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
        [Parameter(Mandatory = $true)]
        [string]$Path1,

        [Parameter(Mandatory = $true)]
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


# ============================================================================
# Create hard link
# ============================================================================

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


# ============================================================================
# Create destination from source
# ============================================================================

function Set-FileFromSource {
    param (
        [Parameter(Mandatory = $true)]
        [string]$Path,

        [Parameter(Mandatory = $true)]
        [string]$Target,

        [string]$FileName
    )

    try {

        New-HardLink `
            -Path $Path `
            -Target $Target

        Write-Log "HARD LINK CREATED."

        return "Linked"
    }
    catch {

        Write-Log "Hard link creation failed; copying file instead."
        Write-Log $_.Exception.Message
    }

    try {

        Copy-Item `
            -LiteralPath $Target `
            -Destination $Path `
            -Force `
            -ErrorAction Stop

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
# Resolve an input source
#
# A compiler path is converted into its MSVC include directory.
# A directory path is used directly.
# ============================================================================

function Resolve-InputSource {
    param (
        [Parameter(Mandatory = $true)]
        [string]$InputPath
    )

    $NormalizedInputPath = $InputPath.Trim().TrimEnd(';')

    if (-not $NormalizedInputPath) {
        throw "Input path is empty."
    }

    if ($NormalizedInputPath -ne $InputPath) {

        Write-Log "Normalized input path:"
        Write-Log "    $NormalizedInputPath"
    }

    try {
        $ResolvedInputPath = [System.IO.Path]::GetFullPath(
            $NormalizedInputPath
        )
    }
    catch {
        throw "Could not resolve input path '$InputPath': $($_.Exception.Message)"
    }

    # ------------------------------------------------------------------------
    # Directory input
    # ------------------------------------------------------------------------

    if (Test-Path -LiteralPath $ResolvedInputPath -PathType Container) {

        Write-Log "Directory input detected."
        Write-Log "    $ResolvedInputPath"

        return [PSCustomObject]@{
            Kind      = 'Directory'
            Root      = $ResolvedInputPath
            InputPath = $ResolvedInputPath
        }
    }

    # ------------------------------------------------------------------------
    # Compiler input
    # ------------------------------------------------------------------------

    if (-not (Test-Path -LiteralPath $ResolvedInputPath -PathType Leaf)) {

        throw "Input path does not exist: $ResolvedInputPath"
    }

    Write-Log "Compiler input detected."
    Write-Log "    $ResolvedInputPath"

    $Current = Get-Item `
        -LiteralPath $ResolvedInputPath `
        -Force

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

        throw "Could not locate MSVC toolchain root for compiler: $ResolvedInputPath"
    }

    $IncludePath = Join-Path `
        $ToolchainRoot `
        "include"

    if (-not (Test-Path -LiteralPath $IncludePath -PathType Container)) {

        throw "MSVC include directory does not exist: $IncludePath"
    }

    Write-Log "Resolved MSVC include directory:"
    Write-Log "    $IncludePath"

    return [PSCustomObject]@{
        Kind      = 'Compiler'
        Root      = $IncludePath
        InputPath = $ResolvedInputPath
    }
}


# ============================================================================
# Find a file below a source root
# ============================================================================

function Get-SourcePath {
    param (
        [Parameter(Mandatory = $true)]
        [string]$SourceRoot,

        [Parameter(Mandatory = $true)]
        [string]$FileName
    )

    # First check the root directly.

    $DirectPath = Join-Path `
        $SourceRoot `
        $FileName

    if (Test-Path -LiteralPath $DirectPath -PathType Leaf) {
        return $DirectPath
    }

    # Then recursively search the source tree.

    $Match = Get-ChildItem `
        -LiteralPath $SourceRoot `
        -Filter $FileName `
        -File `
        -Recurse `
        -ErrorAction SilentlyContinue |
        Select-Object -First 1

    if ($Match) {
        return $Match.FullName
    }

    return $null
}


# ============================================================================
# Provision a destination file
# ============================================================================

function Invoke-FileProvision {
    param (
        [Parameter(Mandatory = $true)]
        [string]$FileName,

        [Parameter(Mandatory = $true)]
        [string]$SourcePath,

        [Parameter(Mandatory = $true)]
        [string]$SourceRoot,

        [Parameter(Mandatory = $true)]
        [string]$DestinationPath
    )

    $LinkPath = Join-Path `
        $DestinationPath `
        $FileName

    Write-Log "------------------------------------------------------------"
    Write-Log "Processing: $FileName"
    Write-Log "Source root:"
    Write-Log "    $SourceRoot"
    Write-Log "Source:"
    Write-Log "    $SourcePath"
    Write-Log "Destination:"
    Write-Log "    $LinkPath"

    if (-not $SourcePath) {

        throw "SOURCE DOES NOT EXIST in provided paths: $FileName"
    }

    # ------------------------------------------------------------------------
    # Destination doesn't exist
    # ------------------------------------------------------------------------

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

        return
    }

    # ------------------------------------------------------------------------
    # Destination already exists
    # ------------------------------------------------------------------------

    $ExistingItem = Get-Item `
        -LiteralPath $LinkPath `
        -Force

    Write-Log "Destination already exists."
    Write-Log "Existing type:"
    Write-Log "    $($ExistingItem.GetType().FullName)"

    if ($ExistingItem.LinkType) {

        Write-Log "Link type:"
        Write-Log "    $($ExistingItem.LinkType)"
    }

    # ------------------------------------------------------------------------
    # Existing symbolic link
    # ------------------------------------------------------------------------

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

        return
    }

    # ------------------------------------------------------------------------
    # Existing regular file
    # ------------------------------------------------------------------------

    if (-not $ExistingItem.PSIsContainer) {

        Write-Log "Existing object is a regular file."
        Write-Log "Checking whether it is already the same file..."

        if (Test-SameFile $SourcePath $LinkPath) {

            Write-Log "YES."
            Write-Log "Existing file is already linked to the source."

            return
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

        return
    }

    # ------------------------------------------------------------------------
    # Existing directory
    # ------------------------------------------------------------------------

    throw "REFUSING TO OVERWRITE EXISTING DIRECTORY: $LinkPath"
}


# ============================================================================
# MAIN WORK
# ============================================================================

try {

    Write-Log ""
    Write-Log "Starting MSVC include processing..."

    # We deliberately have two explicit inputs:
    #
    #   1. CompilerPath -> cl.exe -> MSVC\include
    #   2. UcrtPath     -> Windows SDK UCRT include directory
    #
    # This avoids ambiguous PowerShell array parameter binding.

    $InputPaths = @(
        $CompilerPath
        $UcrtPath
    )

    Write-Log ""
    Write-Log "Input paths:"

    foreach ($InputPath in $InputPaths) {
        Write-Log "    $InputPath"
    }

    # ------------------------------------------------------------------------
    # Resolve all input sources
    # ------------------------------------------------------------------------

    $InputSources = New-Object System.Collections.Generic.List[object]

    foreach ($InputPath in $InputPaths) {

        $ResolvedSource = Resolve-InputSource `
            -InputPath $InputPath

        $AlreadyAdded = $false

        foreach ($ExistingSource in $InputSources) {

            if (
                $ExistingSource.Kind -eq $ResolvedSource.Kind -and
                $ExistingSource.Root -eq $ResolvedSource.Root
            ) {
                $AlreadyAdded = $true
                break
            }
        }

        if (-not $AlreadyAdded) {
            $InputSources.Add($ResolvedSource)
        }
    }

    if ($InputSources.Count -eq 0) {
        throw "No input sources could be resolved."
    }

    Write-Log ""
    Write-Log "Resolved input sources:"

    foreach ($Source in $InputSources) {

        Write-Log "    [$($Source.Kind)] $($Source.Root)"
    }


    # ------------------------------------------------------------------------
    # Destination
    # ------------------------------------------------------------------------

    Write-Log ""
    Write-Log "Destination:"
    Write-Log "    $DestinationPath"

    if (-not (Test-Path -LiteralPath $DestinationPath)) {

        Write-Log "Destination does not exist."
        Write-Log "Creating destination directory..."

        New-Item `
            -ItemType Directory `
            -Path $DestinationPath `
            -Force |
            Out-Null

        Write-Log "Destination created."
    }
    elseif (-not (Test-Path -LiteralPath $DestinationPath -PathType Container)) {

        throw "Destination exists but is not a directory: $DestinationPath"
    }

    Initialize-RunLogFile `
        -Directory $DestinationPath


    # ------------------------------------------------------------------------
    # Process requested files
    # ------------------------------------------------------------------------

    Write-Log ""
    Write-Log "Files to process:"

    foreach ($FileName in $FileNames) {
        Write-Log "    $FileName"
    }

    Write-Log ""

    foreach ($FileName in $FileNames) {

        $SourcePath = $null
        $SourceRoot = $null

        # Search every supplied source root.
        #
        # This means each requested file can come from either:
        #
        #   D:\...\MSVC\...\include
        #
        # or:
        #
        #   C:\Program Files (x86)\Windows Kits\...\ucrt
        #

        foreach ($Source in $InputSources) {

            Write-Log "Searching [$($Source.Kind)] source:"
            Write-Log "    $($Source.Root)"
            Write-Log "For:"
            Write-Log "    $FileName"

            $CandidatePath = Get-SourcePath `
                -SourceRoot $Source.Root `
                -FileName $FileName

            if ($CandidatePath) {

                $SourcePath = $CandidatePath
                $SourceRoot = $Source.Root

                Write-Log "Found:"
                Write-Log "    $SourcePath"

                break
            }
        }

        if (-not $SourcePath) {

            throw "SOURCE DOES NOT EXIST in provided paths: $FileName"
        }

        Invoke-FileProvision `
            -FileName $FileName `
            -SourcePath $SourcePath `
            -SourceRoot $SourceRoot `
            -DestinationPath $DestinationPath
    }


    # ------------------------------------------------------------------------
    # Done
    # ------------------------------------------------------------------------

    Write-Log ""
    Write-Log "============================================================"
    Write-Log "SUCCESS"
    Write-Log "All requested MSVC/UCRT files have been processed."
    Write-Log "============================================================"

    Write-FinalReport `
        -LogDirectory $DestinationPath

    Write-Host ""
    Write-Host "PreBuildTool completed successfully."
    Write-Host "Log: $Script:RunLogFile"

    exit 0
}
catch {

    Write-Log ""
    Write-Log "============================================================"
    Write-Log "ERROR"
    Write-Log $_.Exception.Message
    Write-Log "============================================================"

    # If the destination hasn't been created yet, there may be nowhere
    # to write the log. In that case just emit the error to the console.

    if (
        $DestinationPath -and
        (Test-Path -LiteralPath $DestinationPath -PathType Container)
    ) {
        try {
            Write-FinalReport -LogDirectory $DestinationPath
        }
        catch {
            Write-Host "Failed to write final report: $($_.Exception.Message)"
        }
    }

    Write-Host ""
    Write-Host "PreBuildTool failed: $($_.Exception.Message)"

    exit 1
}