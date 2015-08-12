import socket
import struct

def getbyte(str):
	tbyte = struct.unpack('b', str[0])[0]
	return tbyte, str[1:]

def getint(str):
	tint = struct.unpack('>i', str[:4])[0]
	return tint, str[4:]

def getstring(str):
	for i, v in enumerate(str):
		if v == '\x00':
			break;

	tstr = str[:i]
	return tstr, str[i + 1:]

def setstring(str):
	buf = struct.pack('%ds' % len(str), str)
	buf += struct.pack('b', 0)
	return buf

def getCash(user_index_map, user_index):
	if user_index not in user_index_map:
		user_index_map[user_index] = 100000000

	return user_index_map[user_index]

####################################################################################
def make_packet(body):
	packet = struct.pack('>HiHi', 0, 0, 0, len(body))
	packet += body
	return packet
#####################################################################################
def make_packet_for_ger(type, body):
	packet = struct.pack('<HiH', type, 0, len(body))
	packet += body
	return packet
