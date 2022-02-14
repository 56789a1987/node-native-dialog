const os = require('os');
const fs = require('fs');
const { setEncoding, open, save } = require('../index');

// set this to your encoding if unicode characters are incorrect
os.platform() === 'win32' && setEncoding('cp936');

async function main() {
	const files = await open({
		title: 'Select text files to merge',
		filters: [
			['Text files (*.txt)', '*.txt'],
			['Script files (*.js, *.ts)', '*.js', '*.ts'],
			['All files (*.*)', '*.*'],
		],
		multiple: true
	});

	if (!files) {
		console.log('Open file is cancelled');
		return;
	}

	console.log(files);

	let text = '';
	for (let i = 0; i < files.length; i++) {
		text += fs.readFileSync(files[i], 'utf8');
	}

	const target = await save({
		title: 'Save file',
		name: 'result.txt',
		filters: [
			['Text files (*.txt)', '*.txt'],
			['All files (*.*)', '*.*'],
		],
	});

	if (!target) {
		console.log('Save file is cancelled');
		return;
	}

	console.log(target);

	fs.writeFileSync(target, text, 'utf8');
}

main();
