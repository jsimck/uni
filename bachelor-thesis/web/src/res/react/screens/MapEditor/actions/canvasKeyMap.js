// Keymap handling actions
export const KEYMAP_SET_KEY = 'KEYMAP_SET_KEY';

export function keyMapSetKey(key, value) {
    return {
        type: KEYMAP_SET_KEY,
        value: value,
        key: key
    };
}