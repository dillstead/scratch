from ipaddress import IPv4Address
import socket
import sys

def scan_addresses(start_ip_addr, end_ip_addr, port):
    si = int(IPv4Address(start_ip_addr))
    ei = int(IPv4Address(end_ip_addr))
    cur = min(si, ei)
    
    while cur <= max(si, ei):
        try:
            cur_ip_addr = str(IPv4Address(cur))
            print(f"Trying {cur_ip_addr}:{port}...")
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(1)
            result = sock.connect_ex((cur_ip_addr, port))
            if result == 0:
                print(f"Server is listening on {cur_ip_addr}:{port}")
        except socket.error as e:
            print(f"Error: {e}")
        finally:
            sock.close()
        cur += 1

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python check_pi <start_ip> <end_ip> <port>")
        sys.exit(1)

start_ip_addr = sys.argv[1]
end_ip_addr = sys.argv[2]
port = int(sys.argv[3])

scan_addresses(start_ip_addr, end_ip_addr, port)
