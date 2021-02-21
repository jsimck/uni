import * as actions from '../actions/editModal'

const initialState = {
    visible: false,
    event: null
};

const editModal = (state = initialState, action) => {
    switch (action.type) {
        case actions.OPEN_MODAL:
            return {
                ...state,
                event: action.event,
                visible: true
            };

        case actions.CLOSE_MODAL:
            return {
                ...state,
                visible: false
            };

        default:
            return state;
    }
};

export default editModal