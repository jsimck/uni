import { createStructuredSelector } from 'reselect'

export const notificationsSelector = createStructuredSelector({
    notifications: state => state.notifications.notifications
});