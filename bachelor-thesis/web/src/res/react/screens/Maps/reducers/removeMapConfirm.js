import * as actions from '../actions/removeMapConfirm'

const initialState = {
    showRemoveConfirm: false,
    removeId: null
};

const removeMapConfirm = (state = initialState, action) => {
    switch (action.type) {
        case actions.REMOVE_MAP_SHOW_CONFIRM:
            return {
                ...state,
                removeId: action.removeId,
                showRemoveConfirm: true
            };

        case actions.REMOVE_MAP_HIDE_CONFIRM:
            return {
                ...state,
                removeId: null,
                showRemoveConfirm: false
            };

        default:
            return state;
    }
};

export default removeMapConfirm