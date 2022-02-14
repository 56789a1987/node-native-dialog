const os = require('os');
const { setEncoding, color } = require('../index');

// set this to your encoding if unicode characters are incorrect
os.platform() === 'win32' && setEncoding('cp936');

async function main() {
	const value = await color({ full: true, color: 0xff8000 });

	if (value !== null) {
		console.log(`Color: ${value.toString(16).padStart(6, '0')}`);
	} else {
		console.log('Color picker was cancelled');
	}
}

main();
