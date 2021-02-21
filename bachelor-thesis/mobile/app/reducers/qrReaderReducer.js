import * as actions from '../actions/qrReaderActions'
import * as gameModalActions from '../actions/gameModalActions'
import * as alertActions from '../actions/notificationAlertActions'

const initialState = {
    visible: false,
    qrRead: false
};

const qrReader = (state = initialState, action) => {
    switch (action.type) {
        case actions.SHOW_QR_READER:
            return {
                ...state,
                visible: true
            };

        case actions.CLOSE_QR_READER:
            return {
                ...state,
                visible: false,
                qrRead: false
            };

        case actions.SET_QR_READER_READ:
            return {
                ...state,
                qrRead: action.read
            };

        case alertActions.CLOSE_ALERT:
            return {
                ...state,
                qrRead: false
            };

        case gameModalActions.CLOSE_GAME_MODAL:
            return {
                ...state,
                qrRead: false
            };

        default:
            return state;
    }
};

export default qrReader