import { createStructuredSelector } from 'reselect'

export const loginSelector = createStructuredSelector({
    processing: state => state.login.processing,
    logged: state => state.login.logged
});