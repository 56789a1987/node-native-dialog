const os = require('os');
const { setEncoding, entry, info, warning } = require('../index');

// set this to your encoding if unicode characters are incorrect
os.platform() === 'win32' && setEncoding('cp936');

async function main() {
	const value = await entry('What is your favorite animal?', 'Example input', 'cat');

	if (value !== null) {
		await info(`Your favorite animal is ${value}`, 'Example info');
	} else {
		await warning('You did not enter your favorite animal', 'Example warning');
	}

	console.log(value);
}

main();
