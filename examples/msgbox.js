const os = require('os');
const { setEncoding, question, info, warning } = require('../index');

// set this to your encoding if unicode characters are incorrect
os.platform() === 'win32' && setEncoding('cp936');

async function main() {
	const value = await question('Press a button', 'Example question');

	if (value) {
		await info('You pressed OK', 'Example info');
	} else {
		await warning('You pressed Cancel', 'Example warning');
	}

	console.log(value);
}

main();
