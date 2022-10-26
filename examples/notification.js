const os = require('os');
const { setEncoding, notification } = require('../index');

// set this to your encoding if unicode characters are incorrect
os.platform() === 'win32' && setEncoding('cp936');

notification('This is a notification message', 'Notification', 'info');
