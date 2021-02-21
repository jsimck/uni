import * as actions from '../actions/statusBarActions'

const initialState = {
    networkActivity: false
};

const statusBar = (state = initialState, action) => {
    switch (action.type) {
        case actions.ACTIVATE_NETWORK_ACTIVITY:
            return {
                ...state,
                networkActivity: true
            };

        case actions.DEACTIVATE_NETWORK_ACTIVITY:
            return {
                ...state,
                networkActivity: false
            };

        default:
            return state;
    }
};

export default statusBar