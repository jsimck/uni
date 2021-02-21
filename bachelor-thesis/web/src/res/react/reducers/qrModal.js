import * as actions from '../actions/qrModal'

const initialState = {
    data: '',
    visible: false
};

const notifications = (state = initialState, action) => {
    switch (action.type) {
        case actions.QR_MODAL_OPEN:
            return {
                ...state,
                visible: true,
                data: action.data
            };

        case actions.QR_MODAL_CLOSE:
            return {
                ...state,
                visible: false,
                data: ''
            };

        default:
            return state;
    }
};

export default notifications