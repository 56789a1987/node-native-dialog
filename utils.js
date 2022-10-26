const { spawn } = require('child_process');
const iconv = require('iconv-lite');

let _encoding = undefined;
const decode = buffer => _encoding ? iconv.decode(buffer, _encoding) : buffer.toString();

module.exports.setEncoding = (encoding) => {
	_encoding = encoding;
}

module.exports.runAsync = (command, args) => new Promise((resolve, reject) => {
	let stdout = '', stderr = '';
	const proc = spawn(command, args, { encoding: 'buffer' });
	proc.stdout.on('data', data => stdout += decode(data));
	proc.stderr.on('data', data => stderr += decode(data));
	proc.on('error', reject);
	proc.on('exit', code => {
		stdout = stdout.replace(/\r?\n$/, '');
		if (code === 0 || code === 1) {
			resolve({ code, stdout, stderr });
		} else {
			reject(`Process exit with unexpected return code (${code})`);
		}
	});
});

module.exports.runWithStdin = (command, args) => {
	let stdout = '', stderr = '';
	const proc = spawn(command, args, { encoding: 'buffer' });
	proc.stdout.on('data', data => stdout += decode(data));
	proc.stderr.on('data', data => stderr += decode(data));
	return {
		promise: new Promise((resolve, reject) => {
			proc.on('error', reject);
			proc.on('exit', code => {
				stdout = stdout.replace(/\r?\n$/, '');
				if (code === 0 || code === 1) {
					resolve({ code, stdout, stderr });
				} else {
					reject(`Process exit with unexpected return code (${code})`);
				}
			});
		}),
		stdinWrite: (text) => proc.stdin.write(text + '\n'),
		stdinEnd: () => proc.stdin.end()
	};
};

const unsupported = () => {
	throw new Error('Your OS platform is not yet supported');
};

module.exports.unsupportedPlatform = {
	info: unsupported,
	error: unsupported,
	warning: unsupported,
	question: unsupported,
	entry: unsupported,
	password: unsupported,
	color: unsupported,
	open: unsupported,
	save: unsupported,
	directory: unsupported,
	progress: unsupported,
	notification: unsupported,
};
