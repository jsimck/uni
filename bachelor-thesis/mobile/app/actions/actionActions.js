// Set Actions actions
export const SET_ACTION = 'SET_ACTION';

export function setAction(action) {
    return {
        type: SET_ACTION,
        action: action
    };
}