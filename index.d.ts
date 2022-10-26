export interface ColorDialogOptions {
	full?: boolean;
	color?: number;
	templates?: number[];
}

export interface FileDialogOptions {
	name?: string;
	title?: string;
	initial?: string;
	filters?: string[][];
}

export interface OpenDialogOptionsMulti extends FileDialogOptions {
	multiple: true;
}

export interface DirectoryDialogOptions {
	title?: string;
	initial?: string;
}

export interface ProgressDialogOptions {
	title: string;
	text: string;
	value?: number;
	indeterminate?: boolean;
	noCancel?: boolean;
}

export interface ProgressDialogInstance {
	promise: Promise<boolean>;
	setText: (text: string) => void;
	setValue: (value: number) => void;
}

export function setEncoding(encoding?: string | undefined): void;

export function info(text: string, title: string): Promise<void>;
export function error(text: string, title: string): Promise<void>;
export function warning(text: string, title: string): Promise<void>;
export function question(text: string, title: string): Promise<boolean>;

export function entry(text: string, title: string, _default?: string): Promise<string | null>;
export function password(text: string, title: string, _default?: string): Promise<string | null>;

export function color(options: ColorDialogOptions): Promise<number | null>;

export function open(options: FileDialogOptions): Promise<string | null>;
export function open(options: OpenDialogOptionsMulti): Promise<string[] | null>;
export function save(options: FileDialogOptions): Promise<string | null>;
export function directory(options: DirectoryDialogOptions): Promise<string | null>;

export function progress(options: ProgressDialogOptions): ProgressDialogInstance;
export function notification(text: string, title?: string, icon?: 'info' | 'warning' | 'error'): Promise<void>;
