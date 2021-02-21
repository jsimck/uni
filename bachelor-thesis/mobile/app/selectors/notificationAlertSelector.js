import { createStructuredSelector } from 'reselect'

export const notificationAlertSelector = createStructuredSelector({
    alert: state => state.notificationAlert.alert
});