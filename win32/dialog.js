const path = require('path');
const { runAsync, runWithStdin } = require('../utils');

const command = path.join(__dirname, 'dialog');
const run = (args) => runAsync(command, args);
const runStdin = (args) => runWithStdin(command, args);

const DialogType = {
	Info: '0',
	Error: '1',
	Warning: '2',
	Question: '3',
	Entry: '4',
	Password: '5',
	Color: '6',
	Open: '7',
	Save: '8',
	Directory: '9',
	Progress: 'A',
};

const info = async (text, title) => {
	await run([DialogType.Info, `${text}`, `${title}`]);
};

const error = async (text, title) => {
	await run([DialogType.Error, `${text}`, `${title}`]);
};

const warning = async (text, title) => {
	await run([DialogType.Warning, `${text}`, `${title}`]);
};

const question = async (text, title) => {
	const result = await run([DialogType.Question, `${text}`, `${title}`]);
	return result.code === 0;
};

const entry = async (text, title, _default = '') => {
	const result = await run([DialogType.Entry, `${text}`, `${title}`, `${_default}`]);
	return result.code === 0 ? result.stdout : null;
};

const password = async (text, title, _default = '') => {
	const result = await run([DialogType.Password, `${text}`, `${title}`, `${_default}`]);
	return result.code === 0 ? result.stdout : null;
};

function exchangeEndian(color) {
	color = color >>> 0;
	return ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >>> 16);
}

const color = async (options) => {
	const args = [DialogType.Color];
	if (options.full) {
		args.push('-f');
	}
	if (options.color !== undefined) {
		const color = exchangeEndian(options.color);
		args.push('-c', `${color}`);
	}
	if (Array.isArray(options.templates)) {
		args.push('-t');
		for (let i = 0; i < 16; i++) {
			args.push(`${exchangeEndian(options.templates[i] || 0)}`);
		}
	}
	const result = await run(args);
	return result.code === 0 ? exchangeEndian(parseInt(result.stdout, 10)) : null;
};

function toFilters(filters) {
	let result = '';
	for (let i = 0; i < filters.length; i++) {
		const f = filters[i];
		result += `${f[0]}|${f.slice(1).join(';')}|`;
	}
	return result;
}

const open = async (options) => {
	const args = [DialogType.Open];
	if (options.multiple) {
		args.push('-m');
	}
	if (options.name !== undefined) {
		args.push('-n', `${options.name}`);
	}
	if (options.title !== undefined) {
		args.push('-t', `${options.title}`);
	}
	if (options.initial !== undefined) {
		args.push('-i', `${options.initial}`);
	}
	if (Array.isArray(options.filters)) {
		args.push('-f', toFilters(options.filters));
	}
	const result = await run(args);
	if (result.code !== 0) {
		return null;
	} else if (options.multiple) {
		const selected = result.stdout.split(/\r?\n/);
		const files = [];
		if (selected.length === 1) {
			files.push(selected[0]);
		} else {
			for (let i = 1; i < selected.length; i++) {
				files.push(`${selected[0]}\\${selected[i]}`);
			}
		}
		return files;
	} else {
		return result.stdout;
	}
};

const save = async (options) => {
	const args = [DialogType.Save];
	if (options.name !== undefined) {
		args.push('-n', `${options.name}`);
	}
	if (options.title !== undefined) {
		args.push('-t', `${options.title}`);
	}
	if (options.initial !== undefined) {
		args.push('-i', `${options.initial}`);
	}
	if (Array.isArray(options.filters)) {
		args.push('-f', toFilters(options.filters));
	}
	const result = await run(args);
	return result.code === 0 ? result.stdout : null;
};

const directory = async (options) => {
	const args = [DialogType.Directory];
	if (options.title !== undefined) {
		args.push('-t', `${options.title}`);
	}
	if (options.initial !== undefined) {
		args.push('-i', `${options.initial}`);
	}
	const result = await run(args);
	return result.code === 0 ? result.stdout : null;
};

function clampValue(value) {
	value = value | 0;
	return value < 0 ? 0 : value > 100 ? 100 : value;
}

const progress = (options) => {
	const args = [DialogType.Progress];
	if (options.title !== undefined) {
		args.push('-t', `${options.title}`);
	}
	if (options.text !== undefined) {
		args.push('-m', `${options.text}`);
	}
	if (options.value !== undefined) {
		args.push('-p', clampValue(options.value));
	}
	if (options.indeterminate) {
		args.push('-i');
	}
	if (options.noCancel) {
		args.push('-n');
	}
	const instance = runStdin(args);
	return {
		promise: instance.promise.then(result => result.code === 0),
		setText: (text) => {
			instance.stdinWrite(`# ${text}`.replace(/\r?\n/g, ' '));
		},
		setValue: (value) => {
			instance.stdinWrite(`${clampValue(value)}`);
		}
	};
};

module.exports = {
	info, error, warning, question, entry, password, color, open, save, directory, progress
};
