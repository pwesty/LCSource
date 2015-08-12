import os
import sys
import thread

from common_util import *
from for_korea import *
from for_ger import *

g_ctid_map = {}

def main():
	try:
		listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		listen_socket.bind(('', 2072))
		listen_socket.listen(5)
	except socket.error, e:
		print ('Server Error : %s' % e)
		sys.exit(1)


	while True:
		client_socket, addr = listen_socket.accept()
		if sys.argv[1] == 'kor':
			thread.start_new_thread(handler_client_for_kor, (g_ctid_map, client_socket,addr))
		else:
			thread.start_new_thread(handler_client_for_ger, (g_ctid_map, client_socket,addr))

if __name__ == '__main__':
	if len(sys.argv) != 2:
		print ('Usage: %s [kor | ger]' % sys.argv[0])
		sys.exit(1)

	if sys.argv[1] != 'kor' and sys.argv[1] != 'ger':
		print ('Usage: %s [kor | ger]' % sys.argv[0])
		sys.exit(1)

	main()