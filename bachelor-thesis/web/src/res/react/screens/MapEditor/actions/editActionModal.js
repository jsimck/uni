// Modal Manipulation actions
export const SHOW_EDIT_ACTION_MODAL = 'SHOW_EDIT_ACTION_MODAL';
export const HIDE_EDIT_ACTION_MODAL = 'HIDE_EDIT_ACTION_MODAL';

export function showEditActionModal(index, svg) {
    return {
        type: SHOW_EDIT_ACTION_MODAL,
        index: index,
        svg: svg
    };
}

export function hideEditActionModal() {
    return {
        type: HIDE_EDIT_ACTION_MODAL
    };
}