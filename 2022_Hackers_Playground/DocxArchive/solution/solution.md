# DocxArchive Solution

Docx file structure follows pk zip file structure. Just change file extension to `.zip` make you unzip the docx file.

In docx archive tree structure, you can find `word/embeddings/oleObject1.bin` file which contains attachment file data.

You can traverse ole data with ole unpacker, then extract each ole section with meta data.

Identifying file signature will lead you get the flag.