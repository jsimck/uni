import * as actions from '../actions/loginActions'

const initialState = {
    processing: false,
    logged: false
};

const login = (state = initialState, action) => {
    switch (action.type) {
        case actions.LOGIN_USER_REQUEST:
            return {
                ...state,
                processing: true
            };

        case actions.LOGIN_USER_ERROR:
            return {
                ...state,
                processing: false
            };

        case actions.LOGIN_USER_SUCCESS:
            return {
                ...state,
                processing: false,
                logged: true
            };

        default:
            return state;
    }
};

export default login