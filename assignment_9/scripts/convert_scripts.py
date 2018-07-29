import os, glob
pp = os.path.join
path_file = os.path.realpath(__file__)
path_script_file_linux = os.path.dirname(path_file)
path_script_file_win = pp(path_script_file_linux, '..', 'scripts_win')

print(path_script_file_linux)

for script_in in os.listdir(path_script_file_linux):
	name, ext = os.path.splitext(script_in)
	if ext == '.sh':
		path_src = pp(path_script_file_linux, script_in)
		path_dest = pp(path_script_file_win, name + '.bat')

		print(path_src, '->', path_dest)

		with open(path_src, 'r') as fin:
			lines = [l for l in fin]

		lines = [
			l.replace('build/', 'build\\Debug\\')
			for l in lines
			if not l.strip().startswith('#')
		]

		with open(path_dest, 'w') as fout:
			fout.write('\n'.join(lines))



