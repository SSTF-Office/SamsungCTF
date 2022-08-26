# DocxArchive

Challenge problem for SCTF(Hacker's playground) 2022.

## Category
Misc, Reversing

## Challenge Intention
Check ability to reverse engineer and extract MS word file embedded attachment file.
User shall reverse engineering MS word docx file format and OLE file structure with checking reference and reasonable guessing.

## Challenge Structure
Challenge just provides some hints with challenge detail paragraph and malformed ms word file.
By reading challenge detail paragraph, users can guess that they should recover malformed word file structure.

## Challenge Paragraph
I developed a simple and useful program that attaches a file into word file. But... why I cannot open file?
I thought I developed perfect programm, but it was not true. Wait, where is the source file? I cannot find my attachment file!
I think I need to extract attachment file from word.

## Flag
SCTF{Do-y0u-kn0w-01E-4nd-3mf-forM4t?}