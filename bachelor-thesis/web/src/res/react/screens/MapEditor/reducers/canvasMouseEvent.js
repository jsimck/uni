import * as actions from '../actions/canvasMouseEvent'

export const initialState = {
    eventMouseMove: {}
};

const canvasMouseEvent = (state = initialState, action) => {
    switch (action.type) {
        case actions.SET_MOUSE_MOVE_EVENT:
            return {
                ...state,
                eventMouseMove: action.event
            };

        default:
            return state;
    }
};

export default canvasMouseEvent
