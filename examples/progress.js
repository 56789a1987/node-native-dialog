const os = require('os');
const { setEncoding, warning, progress } = require('../index');

// set this to your encoding if unicode characters are incorrect
os.platform() === 'win32' && setEncoding('cp936');

async function main() {
	const prog = progress({
		title: 'Installing',
		text: 'Preparing for install...',
		value: 0
	});

	setTimeout(() => prog.setValue(10), 1000);
	setTimeout(() => prog.setText('Extracting files...'), 2000);
	setTimeout(() => prog.setValue(20), 3000);
	setTimeout(() => prog.setText('Copying files...'), 4000);
	setTimeout(() => prog.setValue(50), 5000);
	setTimeout(() => prog.setValue(75), 6000);
	setTimeout(() => prog.setText('Registering components...'), 7000);
	setTimeout(() => {
		prog.setText('Finished');
		prog.finish();
	}, 8000);

	const value = await prog.promise;
	if (!value) {
		await warning('Installation was cancelled', 'Warning');
	}
}

main();
