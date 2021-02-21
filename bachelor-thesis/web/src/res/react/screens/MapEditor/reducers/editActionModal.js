import * as actions from '../actions/editActionModal'

const initialState = {
    visible: false,
    index: -1,
    svg: {}
};

const editActionModal = (state = initialState, action) => {
    switch (action.type) {
        case actions.HIDE_EDIT_ACTION_MODAL:
            return {
                ...state,
                visible: false,
                index: -1,
                svg: {}
            };

        case actions.SHOW_EDIT_ACTION_MODAL:
            return {
                ...state,
                visible: true,
                index: action.index,
                svg: action.svg
            };

        default:
            return state;
    }
};

export default editActionModal