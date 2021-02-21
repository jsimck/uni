export const OPEN_MODAL = 'OPEN_MODAL';
export const CLOSE_MODAL = 'CLOSE_MODAL';

export function closeModal() {
    return {
        type: CLOSE_MODAL
    }
}

export function openModal(event) {
    return {
        type: OPEN_MODAL,
        event: event
    }
}