#pragma once
/// <summary>
/// Creates the ProgramHeap with a initial size.
/// InitSize is in pages.
/// when 0 it will create with one page
/// </summary>
/// <param name="initSize"></param>
/// <returns></returns>
bool CreateHeap(size_t initSize);