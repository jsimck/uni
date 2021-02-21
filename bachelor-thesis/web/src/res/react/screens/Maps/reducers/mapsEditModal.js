import * as actions from '../actions/mapsEditModal'

const initialState = {
    isVisible: false,
    map: null
};

const mapsEditModal = (state = initialState, action) => {
    switch (action.type) {
        case actions.SHOW_MAPS_EDIT_MODAL:
            return {
                ...state,
                isVisible: true,
                map: action.map
            };

        case actions.HIDE_MAPS_EDIT_MODAL:
            return {
                ...state,
                isVisible: false
            };

        default:
            return state;
    }
};

export default mapsEditModal