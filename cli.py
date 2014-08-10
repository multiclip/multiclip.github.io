import optparse


KEYS_PATH = '/dev/multiclip/'

def parseArgs():
	parser = optparse.OptionParser()
	

	parser.add_option('-k','--key',
						dest = 'key',
						type = 'string',
						action = 'store',
						metavar = 'KEY',
						help = 'which key to see what is stored'
						)
	


	parser.add_option('-a','--all',
						dest = 'all',
						action = 'store_true',
						help = 'see what is stored in all keys'
						)
	


	return parser.parse_args()

def get( key ):
	key_file = open(KEYS_PATH+chr(key), 'r')
	return key_file.read()

def get_all():
	result = []
	for key in range(1,128):
		value = get(key)
		if value:
			result += value
	return result

def main():
	options, remainder = parseArgs()
	



if __name__== "__main__":
	main()
