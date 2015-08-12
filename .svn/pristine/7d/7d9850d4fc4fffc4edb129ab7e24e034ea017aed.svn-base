import os
import sys
import hexdump
import struct
import socket

from common_util import *

HEADER_DUMP_STRING_GER = '<HiH'
HEADER_SIZE_FOR_GER = struct.calcsize(HEADER_DUMP_STRING_GER)

def hander_client_for_ger_read_header(client_socket):
	data = ''
	recv_length = HEADER_SIZE_FOR_GER
	while recv_length > 0:
		t = client_socket.recv(recv_length)
		if not t:
			return None

		data += t
		recv_length -= len(t)

	print (hexdump.hexdump(data))

	ptype, psn, psize = struct.unpack(HEADER_DUMP_STRING_GER, data)
	return ptype, psize

def handler_client_for_ger_read_body(client_socket, body_size):
	data = ''
	recv_length = body_size
	while recv_length > 0:
		t = client_socket.recv(recv_length)
		if not t:
			return None

		data += t
		recv_length -= len(t)

	print (hexdump.hexdump(data))
	return data


def handler_client_for_ger_process(type, packet_body, user_index_map, ctid_map, client_socket):
	if type == 1100:
		# first connect info
		protocol_version, packet_body = getstring(packet_body)
		server_no, packet_body = getbyte(packet_body)
		game_id, packet_body = getbyte(packet_body)
		print ('protocol_version : %s : server_no : %s : game_id : %s' % (protocol_version, server_no, game_id))
		return

	elif type == 1700:
		# now cash
		user_id, packet_body = getstring(packet_body)
		cpid, packet_body = getint(packet_body)
		user_index, packet_body = getint(packet_body)
		print ('user_index : %s : user_id : %s : cpid : %s' % (user_index, user_id, cpid))
		######################################################
		cash = getCash(user_index_map, user_index)

		body = struct.pack('b', 0) # return code
		body += setstring(user_id)
		body += struct.pack('b', cpid)
		body += struct.pack('>i', user_index)
		body += struct.pack('>i', cash)
		body += struct.pack('>i', cash // 2)
		rpacket = make_packet_for_ger(type + 1000, body)
		client_socket.send(rpacket)
		return

	elif type == 1720:
		# can buy
		user_id, packet_body = getstring(packet_body)
		cpid, packet_body = getint(packet_body)
		user_index, packet_body = getint(packet_body)
		ip, packet_body = getstring(packet_body)
		count, packet_body = getbyte(packet_body)
		ctid = []
		for i in range(count):
			tctid, packet_body = getint(packet_body)
			ctid.append(tctid)
		print ('user_id : %s : cpid : %s : user_index : %s : ip : %s :  count : %s' % (user_id, cpid, user_index, ip, count))
		for i, v in enumerate(ctid):
			print ('ctid[%s] = %s' % (i, v))
		################################################
		cash = getCash(user_index_map, user_index)

		body = struct.pack('b', 0) # return code
		body += setstring(user_id)
		body += struct.pack('b', cpid)
		body += struct.pack('>i', user_index)
		body += struct.pack('b', count)
		for v in ctid:
			body += struct.pack('>i', v)
		body += struct.pack('>i', cash)
		body += struct.pack('>i', cash // 2)
		rpacket = make_packet_for_ger(type + 1000, body)
		client_socket.send(rpacket)
		return

	elif type == 1740:
		user_id, packet_body = getstring(packet_body)
		cpid, packet_body = getint(packet_body)
		user_index, packet_body = getint(packet_body)
		ip, packet_body = getstring(packet_body)
		count, packet_body = getbyte(packet_body)
		ctid = []
		for i in range(count):
			tctid, packet_body = getint(packet_body)
			ctid.append(tctid)
		print ('user_id : %s : cpid : %s : user_index : %s : ip : %s : count : %s' % (user_id, cpid, user_index, ip, count))
		for i, v in enumerate(ctid):
			print ('ctid[%s] = %s' % (i, v))
		################################################
		cash = getCash(user_index_map, user_index)

		body = struct.pack('b', 0) # return code
		body += setstring(user_id)
		body += struct.pack('b', cpid)
		body += struct.pack('>i', user_index)
		body += struct.pack('b', len(ctid))
		for v in ctid:
			body += struct.pack('>i', v)
		body += struct.pack('>i', cash)
		body += struct.pack('>i', cash // 2)
		rpacket = make_packet_for_ger(type + 1000, body)
		client_socket.send(rpacket)
		return

	elif type == 8:
		# alive packet
		return

def handler_client_for_ger(ctid_map, client_socket, addr):
	print ('Conntion from %s : %s' % addr)

	user_index_map = {}

	exit_flag = False
	while True:
		ptype, packet_size = hander_client_for_ger_read_header(client_socket)
		if packet_size == None:
			break

		packet_body = handler_client_for_ger_read_body(client_socket, packet_size)
		if packet_body == None:
			break

		print ('type : %s' % ptype)

		handler_client_for_ger_process(ptype, packet_body, user_index_map, ctid_map, client_socket)

	client_socket.close()

	print ('Disconntion from %s : %s' % addr)

