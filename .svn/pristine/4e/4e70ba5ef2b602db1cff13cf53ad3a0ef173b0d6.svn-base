import os
import sys
import hexdump
import struct
import socket

from common_util import *

HEADER_DUMP_STRING = '>HiHi'
HEADER_SIZE_FOR_KOR = struct.calcsize(HEADER_DUMP_STRING)

def hander_client_for_kor_read_header(client_socket):
	data = ''
	recv_length = HEADER_SIZE_FOR_KOR
	while recv_length > 0:
		t = client_socket.recv(recv_length)
		if not t:
			return None

		data += t
		recv_length -= len(t)

	print (hexdump.hexdump(data))

	preliable, pseq, pid, psize = struct.unpack(HEADER_DUMP_STRING, data)
	return psize

def handler_client_for_kor_read_body(client_socket, body_size):
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


def handler_client_for_kor_process(type, packet_body, user_index_map, ctid_map, client_socket):
	if type == 1:
		# first connect info
		billing_str, packet_body = getstring(packet_body)
		server_no, packet_body = getbyte(packet_body)
		game_id, packet_body = getbyte(packet_body)
		print ('str : %s : server_no : %s : game_id : %s' % (billing_str, server_no, game_id))
		return

	elif type == 2:
		# now cash
		game_id, packet_body = getbyte(packet_body)
		user_index, packet_body = getint(packet_body)
		user_id, packet_body = getstring(packet_body)
		cpid, packet_body = getint(packet_body)
		print ('game_id : %s : user_index : %s : user_id : %s : cpid : %s' % (game_id, user_index, user_id, cpid))
		######################################################
		cash = getCash(user_index_map, user_index)

		body = struct.pack('b', type + 1) # main type
		body += struct.pack('b', 0) # return code
		body += struct.pack('b', game_id)
		body += struct.pack('>i', user_index)
		body += struct.pack('>i', cpid)
		body += struct.pack('>i', cash)
		rpacket = make_packet(body)
		client_socket.send(rpacket)
		return

	elif type == 4:
		# can buy
		game_id, packet_body = getbyte(packet_body)
		user_index, packet_body = getint(packet_body)
		user_id, packet_body = getstring(packet_body)
		cpid, packet_body = getint(packet_body)
		count, packet_body = getbyte(packet_body)
		ctid = []
		for i in range(count):
			tctid, packet_body = getint(packet_body)
			ctid.append(tctid)
		request_cash, packet_body = getint(packet_body)
		print ('game_id : %s : user_index : %s : user_id : %s : cpid : %s : count : %s' % (game_id, user_index, user_id, cpid, count))
		for i, v in enumerate(ctid):
			print ('ctid[%s] = %s' % (i, v))
		print ('request_cash = %s' % request_cash)
		################################################
		cash = getCash(user_index_map, user_index)

		body = struct.pack('b', type + 1)
		body += struct.pack('b', 0) # return code
		body += struct.pack('b', game_id)
		body += struct.pack('>i', user_index)
		body += struct.pack('>i', cpid)
		body += struct.pack('b', len(ctid))
		for v in ctid:
			body += struct.pack('>i', v)
		body += struct.pack('>i', cash)
		rpacket = make_packet(body)
		client_socket.send(rpacket)
		return

	elif type == 6:
		serial, packet_body = getint(packet_body)
		game_id, packet_body = getbyte(packet_body)
		user_index, packet_body = getint(packet_body)
		user_id, packet_body = getstring(packet_body)
		cpid, packet_body = getint(packet_body)
		ip, packet_body = getstring(packet_body)
		count, packet_body = getbyte(packet_body)
		ctid = []
		for i in range(count):
			tctid, packet_body = getint(packet_body)
			ctid.append(tctid)
		print ('serial : %s : game_id : %s : user_index : %s : user_id : %s : cpid : %s : count : %s' % (serial, game_id, user_index, user_id, cpid, count))
		for i, v in enumerate(ctid):
			print ('ctid[%s] = %s' % (i, v))
		################################################
		cash = getCash(user_index_map, user_index)

		body = struct.pack('b', type + 1)
		body += struct.pack('>i', serial)
		body += struct.pack('b', 0) # return code
		body += struct.pack('b', game_id)
		body += struct.pack('>i', user_index)
		body += struct.pack('>i', cpid)
		body += struct.pack('b', len(ctid))
		for v in ctid:
			body += struct.pack('>i', v)
		body += struct.pack('>i', cash)
		rpacket = make_packet(body)

		print (hexdump.hexdump(rpacket))

		client_socket.send(rpacket)
		return

	elif type == 8:
		# alive packet
		return

def handler_client_for_kor(ctid_map, client_socket, addr):
	print ('Conntion from %s : %s' % addr)

	user_index_map = {}

	exit_flag = False
	while True:
		packet_size = hander_client_for_kor_read_header(client_socket)
		if packet_size == None:
			break

		packet_body = handler_client_for_kor_read_body(client_socket, packet_size)
		if packet_body == None:
			break

		type, packet_body = getbyte(packet_body)
		print ('type : %s' % type)

		handler_client_for_kor_process(type, packet_body, user_index_map, ctid_map, client_socket)

	client_socket.close()

	print ('Disconntion from %s : %s' % addr)

