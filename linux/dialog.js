const { runAsync } = require('../utils');

const run = (args) => runAsync('zenity', args);

const info = async (text, title) => {
	await run([
		'--info', '--no-wrap', '--no-markup',
		'--text', `${text}`,
		'--title', `${title}`,
	]);
};

const error = async (text, title) => {
	await run([
		'--error', '--no-wrap', '--no-markup',
		'--text', `${text}`,
		'--title', `${title}`,
	]);
};

const warning = async (text, title) => {
	await run([
		'--warning', '--no-wrap', '--no-markup',
		'--text', `${text}`,
		'--title', `${title}`,
	]);
};

const question = async (text, title) => {
	const result = await run([
		'--question', '--no-wrap', '--no-markup',
		'--text', `${text}`,
		'--title', `${title}`,
	]);
	return result.code === 0;
};

const entry = async (text, title, _default = '') => {
	const result = await run([
		'--entry',
		'--text', `${text}`,
		'--title', `${title}`,
		'--entry-text', `${_default}`,
	]);
	return result.code === 0 ? result.stdout : null;
};

const password = async (text, title, _default = '') => {
	const result = await run([
		'--entry', '--hide-text',
		'--text', `${text}`,
		'--title', `${title}`,
		'--entry-text', `${_default}`,
	]);
	return result.code === 0 ? result.stdout : null;
};

const color = async (options) => {
	const args = ['--color-selection'];
	if (!options.full) {
		args.push('--show-palette');
	}
	if (options.color !== undefined) {
		const color = (options.color >>> 0).toString(16).padStart(6, '0');
		args.push('--color', `#${color}`);
	}
	const result = await run(args);
	if (result.code === 0) {
		const [x, y, z] = result.stdout.match(/\d+/g);
		const r = parseInt(x, 10), g = parseInt(y, 10), b = parseInt(z, 10);
		return r | (g << 8) | (b << 16);
	} else {
		return null;
	}
};

function addFilters(args, filters) {
	for (let i = 0; i < filters.length; i++) {
		const f = filters[i];
		args.push('--file-filter', `${f[0]} | ${f.slice(1).join(' ')}`);
	}
}

const open = async (options) => {
	const args = ['--file-selection'];
	if (options.multiple) {
		args.push('--multiple');
	}
	if (options.name !== undefined) {
		args.push('--filename', `${options.name}`);
	}
	if (options.title !== undefined) {
		args.push('--title', `${options.title}`);
	}
	if (Array.isArray(options.filters)) {
		addFilters(args, options.filters);
	}
	const result = await run(args);
	if (result.code !== 0) {
		return null;
	} else if (options.multiple) {
		return result.stdout.split('|');
	} else {
		return result.stdout;
	}
};

const save = async (options) => {
	const args = ['--file-selection', '--save', '--confirm-overwrite'];
	if (options.name !== undefined) {
		args.push('--filename', `${options.name}`);
	}
	if (options.title !== undefined) {
		args.push('--title', `${options.title}`);
	}
	if (Array.isArray(options.filters)) {
		addFilters(args, options.filters);
	}
	const result = await run(args);
	return result.code === 0 ? result.stdout : null;
};

const directory = async (options) => {
	const args = ['--file-selection', '--directory'];
	if (options.initial !== undefined) {
		args.push('--filename', `${options.initial}`);
	}
	if (options.title !== undefined) {
		args.push('--title', `${options.title}`);
	}
	const result = await run(args);
	return result.code === 0 ? result.stdout : null;
};

module.exports = {
	info, error, warning, question, entry, password, color, open, save, directory
};
