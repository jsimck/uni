// Open&close Modal actions
export const OPEN_EVENTS_GAME_MODAL = 'OPEN_EVENTS_GAME_MODAL';
export const CLOSE_EVENTS_GAME_MODAL = 'CLOSE_EVENTS_GAME_MODAL';

export function openEventsGameModal(eventId) {
    return {
        eventId: eventId,
        type: OPEN_EVENTS_GAME_MODAL
    };
}

export function closeEventsGameModal() {
    return {
        type: CLOSE_EVENTS_GAME_MODAL
    };
}