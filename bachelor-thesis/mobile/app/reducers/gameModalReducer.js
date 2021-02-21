import * as actions from '../actions/gameModalActions'

const initialState = {
    visible: false,
    event: null,
    type: '',
    title: '',
    text: ''
};

const games = (state = initialState, action) => {
    switch (action.type) {
        case actions.OPEN_GAME_MODAL:
            return {
                ...state,
                visible: true,
                event: action.event,
                title: action.title,
                text: action.text,
                type: action.imgType
            };

        case actions.CLOSE_GAME_MODAL:
            return {
                ...state,
                visible: false
            };

        default:
            return state;
    }
};

export default games