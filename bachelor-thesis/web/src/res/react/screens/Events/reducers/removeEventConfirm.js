import * as actions from '../actions/removeEventConfirm'

const initialState = {
    removeConfirm: false,
    removeId: null
};

const removeEventConfirm = (state = initialState, action) => {
    switch (action.type) {
        case actions.REMOVE_EVENT_CONFIRM_CLOSE:
            return {
                ...state,
                removeConfirm: false,
                removeId: null
            };

        case actions.REMOVE_EVENT_CONFIRM_SHOW:
            return {
                ...state,
                removeConfirm: true,
                removeId: action.id
            };

        default:
            return state;
    }
};

export default removeEventConfirm