// Event handling actions
export const SET_MOUSE_MOVE_EVENT = 'SET_MOUSE_MOVE_EVENT';

export function setMouseMoveEvent(event) {
    return {
        type: SET_MOUSE_MOVE_EVENT,
        event: event
    };
}