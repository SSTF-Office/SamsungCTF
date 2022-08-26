#!/usr/bin/env python

import zipfile
import olefile
import struct

DOCX_PATH = "../prob/RecoverMe.docx"

def extractFileContents(binData):
    start = 6
    end = start
    while end < len(binData) and binData[end] != 0:
        end = end + 1
    fileName = binData[start:end]

    start = end + 1
    end = start
    while end < len(binData) and binData[end] != 0:
        end = end + 1
    filePath = binData[start:end]

    start = end + 1 + 4 + 4
    end = start
    while end < len(binData) and binData[end] != 0:
        end = end + 1
    tempFilePath = binData[start:end]

    start = end + 1
    end = start + 4

    contentLength = struct.unpack("<I", binData[start:end])[0]
    
    start = end
    end = start + contentLength

    contents = binData[start:end]
    # print (fileName)
    # print (filePath)
    # print (tempFilePath)
    # print (contentLength)
    return contents

oleSectionData = None
with zipfile.ZipFile(DOCX_PATH) as docx:
    with docx.open("word/embeddings/oleObject1.bin") as oleObject:
        ole = olefile.OleFileIO(oleObject)
        oleSectionData = ole.openstream("\x01ole10Native").read()

contents = extractFileContents(oleSectionData)
with open("flag-file.emf", "rb") as f:
    realContents = f.read()
    if contents == realContents:
        print ("File is correctly extracted")
        # with open("recovered.emf", "wb") as f:
        #     f.write(contents)
        #     sys.exit()
    else:
        print ("File not matched!")
 