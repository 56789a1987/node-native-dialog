const os = require('os');
const fs = require('fs');
const { setEncoding, directory } = require('../index');

// set this to your encoding if unicode characters are incorrect
os.platform() === 'win32' && setEncoding('cp936');

async function main() {
	const value = await directory({ title: 'Select a directory and list files in it' });

	if (value !== null) {
		console.log(`Directory: "${value}"`);
		console.log(fs.readdirSync(value));
	} else {
		console.log('Choose directory was cancelled');
	}
}

main();
