import socket

admin_id = 'w44s_adm1n'
admin_pw = 'w44s_p455w0rd'
flag = 'SCTF{what_host_r_u_looking_for}'
blacklist = ['fuck', 'shit', '127.0.0.1', '\' or 1=1', '1=1', '\' or']

admin_ip = ['127.0.0.1', '143.248.231.67', '52.78.141.191']
ip = socket.gethostbyname(socket.gethostname()) # get local ip address
ip = ip[:ip.rfind('.')]+'.1' # docker gateway ip
admin_ip.append(ip)
# change these values
domain = 'sctfwaas.com'
front_port = 8080
back_port = 55555

# domain:port setup
domain += ':' + str(front_port)


