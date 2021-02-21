// Edit modal Actions
export const SHOW_MAPS_EDIT_MODAL = 'SHOW_MAPS_EDIT_MODAL';
export const HIDE_MAPS_EDIT_MODAL = 'HIDE_MAPS_EDIT_MODAL';

export function showMapsEditModal(selectedMap) {
    return {
        type: SHOW_MAPS_EDIT_MODAL,
        map: selectedMap
    }
}

export function hideMapsEditModal() {
    return {
        type: HIDE_MAPS_EDIT_MODAL
    }
}