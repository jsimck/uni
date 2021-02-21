import { createStructuredSelector } from 'reselect'

export const actionSelector = createStructuredSelector({
    action: state => state.action.action
});