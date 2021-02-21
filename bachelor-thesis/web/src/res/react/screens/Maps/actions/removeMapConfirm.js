// Edit modal Actions
export const REMOVE_MAP_SHOW_CONFIRM = 'REMOVE_MAP_SHOW_CONFIRM';
export const REMOVE_MAP_HIDE_CONFIRM = 'REMOVE_MAP_HIDE_CONFIRM';

export function removeMapShowConfirm(id) {
    return {
        type: REMOVE_MAP_SHOW_CONFIRM,
        removeId: id
    };
}

export function removeMapHideConfirm() {
    return {
        type: REMOVE_MAP_HIDE_CONFIRM
    };
}