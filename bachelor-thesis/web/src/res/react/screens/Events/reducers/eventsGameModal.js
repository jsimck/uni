import * as actions from '../actions/eventsGameModal'

const initialState = {
    eventId: null,
    visible: false
};

const eventsGameModal = (state = initialState, action) => {
    switch (action.type) {
        case actions.OPEN_EVENTS_GAME_MODAL:
            return {
                ...state,
                eventId: action.eventId,
                visible: true
            };

        case actions.CLOSE_EVENTS_GAME_MODAL:
            return {
                ...state,
                eventId: null,
                visible: false
            };

        default:
            return state;
    }
};

export default eventsGameModal