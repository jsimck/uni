// Push Notification action
export const PUSH_NOTIFICATION = 'PUSH_NOTIFICATION';

export function pushNotification(notifType, message) {
    return {
        type: PUSH_NOTIFICATION,
        notifType: notifType,
        message: message
    };
}

// Remove Notification action
export const REMOVE_NOTIFICATION = 'REMOVE_NOTIFICATION';

export function removeNotification(id) {
    return {
        type: REMOVE_NOTIFICATION,
        id: id
    };
}