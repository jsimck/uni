// Game modal actions
export const OPEN_GAME_MODAL = 'OPEN_GAME_MODAL';
export const CLOSE_GAME_MODAL = 'CLOSE_GAME_MODAL';

// Image types
export const GAME_MODAL_IMG_ERROR = 'GAME_MODAL_IMG_ERROR';
export const GAME_MODAL_IMG_INFO = 'GAME_MODAL_IMG_INFO';
export const GAME_MODAL_IMG_SCAN = 'GAME_MODAL_IMG_SCAN';
export const GAME_MODAL_IMG_WON = 'GAME_MODAL_IMG_WON';

export function openGameModal(title, text, imgType, event) {
    return {
        type: OPEN_GAME_MODAL,
        title: title,
        text: text,
        imgType: imgType,
        event: event
    };
}

export function closeGameModal() {
    return {
        type: CLOSE_GAME_MODAL
    };
}