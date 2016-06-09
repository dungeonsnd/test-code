#!/bin/env python
# -*- coding: utf-8 -*-

import json
import zlib
import struct
import sys
sys.path.append('./')
from pys_encryption import *

"""
-----header----
Magic : 8B , 1Q2W3E4R
body length : 4B , body length

-----body----
AES Length: 4B
TDES Length: 4B
checksum: 4B , left checksum
Uncompress  Length: 4B

extra data : 2B
actual body : json body.

padding bytes for aes encryption : 16-len(input)%16
"""


def AddMagic(input):
    return '1Q2W3E4R'+input
def TestMagic(input):
    if '1Q2W3E4R'!=input[0:8]:
        return None
    else:
        return input[8:]

def PackPackage(input):
    ret =AddMagic(struct.pack('!i',len(input))+input)
    return ret
def GetBodyLenByHeader(header):
    return struct.unpack('!i',TestMagic(header))[0]

    
def AESEncrypt(input,key):
    cryto =AESEncryption(key)
    length =struct.pack('!i',len(input))  # Input strings must be a multiple of 16 in length
    t =len(input)%16
    if t!=0:
        input =input+'_'*(16-t)
    ciphertext = cryto.Encrypt(input)
    return length+ciphertext
    
def AESDecrypt(input,key):
    cryto =AESEncryption(key)
    length =struct.unpack('!i',input[0:4])[0]
    plain = cryto.Decrypt(input[4:])
    return plain[0:length]
    
    
def TDESEncrypt(input):
    cryto =TDESEncryption()
    length =struct.pack('!i',len(input))  # Input strings must be a multiple of 16 in length
    t =len(input)%16
    if t!=0:
        input =input+'_'*(16-t)
    ciphertext = cryto.Encrypt(input)
    return length+ciphertext
    
def TDESDecrypt(input):
    cryto =TDESEncryption()
    length =struct.unpack('!i',input[0:4])[0]
    plain = cryto.Decrypt(input[4:])
    return plain[0:length]
    

def AddChecksum(input):
    sum =zlib.adler32(input)
    return struct.pack('!i',sum)+input
    
def TestChecksum(input):
    sumPackage =struct.unpack('!i',input[0:4])[0]
    sumCal =zlib.adler32(input[4:])
    if sumCal!=sumPackage:
        print 'TestChecksum:  sumPackage=%s != sumCal=%s'%(sumPackage,sumCal)
        return None
    else:
        return input[4:]

def CompressData(input):
    length =struct.pack('!i',len(input))
    data =zlib.compress(input,9)
    return length+data
def UncompressData(input):
    length =struct.unpack('!i',input[0:4])[0]
    data = zlib.decompress(input[4:])
    return data
    
def SerializeFilter(data,key):
    data =json.dumps(data)
#    data =CompressData(data)
#    data =AddChecksum(data)
#    data =TDESEncrypt(data)
#    data =AESEncrypt(data,key)
    return data
def DeserializeFilter(data,key):
#    data =AESDecrypt(data,key)
#    data =TDESDecrypt(data)
#    data =TestChecksum(data)
#    if not data:
#        return None
#    data =UncompressData(data)
    data =json.loads(data)
    return data


def GenInitSessionData():
    return {u'sessionkey':GenRandomData()}
    
def PackInitSession(data):
    data =json.dumps(data)
#    data =zlib.compress(data,9)
#    data =AddChecksum(data)
#    data =TDESEncrypt(data)
    return data
def UnpackInitSession(data):
#    data =TDESDecrypt(data)
#    data =TestChecksum(data)
#    if not data:
#        return None
#    data =zlib.decompress(data)
    data =json.loads(data)
    return data
    
