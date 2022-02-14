const os = require('os');
const { unsupportedPlatform, setEncoding } = require('./utils');

let dialogs;

try {
	dialogs = require(`./${os.platform()}/dialog`);
} catch (e) {
	if (/^Cannot find module/.test(e.message)) {
		dialogs = unsupportedPlatform;
	} else {
		throw e;
	}
}

module.exports = {
	...dialogs,
	setEncoding,
};
