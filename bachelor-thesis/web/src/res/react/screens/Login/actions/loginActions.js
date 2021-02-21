require('es6-promise').polyfill();
import fetch from 'isomorphic-fetch'
import jsCookie from 'js-cookie'
import { pushNotification } from '../../../actions/notifications'

// Actions to handle user login
export const LOGIN_USER_REQUEST = 'LOGIN_USER_REQUEST';
export const LOGIN_USER_ERROR = 'LOGIN_USER_ERROR';
export const LOGIN_USER_SUCCESS = 'LOGIN_USER_SUCCESS';

export function loginUserRequest() {
    return {
        type: LOGIN_USER_REQUEST
    };
}

export function loginUserError() {
    return {
        type: LOGIN_USER_ERROR
    };
}

export function loginUserSuccess() {
    return {
        type: LOGIN_USER_SUCCESS
    };
}

export function loginUser(login, password, remember) {
    return function (dispatch) {
        dispatch(loginUserRequest());

        return fetch('/', {
            method: 'POST',
            credentials: 'include',
            headers: {
                'Content-Type': 'application/json',
                'X-CSRFToken': jsCookie.get('csrftoken')
            },
            body: JSON.stringify({
                    login: login,
                    password: password,
                    remember: remember
                })
            })
            .then(response => {
                if (response.status === 404) {
                    throw new Error('Chyba přihlášení');
                }

                return response.json();
            })
            .then(json => {
                if (!json.success) {
                    throw new Error(json.message);
                }

                dispatch(loginUserSuccess());
                dispatch(pushNotification('success', 'Uživatel byl úspěšně přihlášen'));
            })
            .catch(error => {
                dispatch(loginUserError());
                dispatch(pushNotification('danger', error.toString()));
            });
    }
}
