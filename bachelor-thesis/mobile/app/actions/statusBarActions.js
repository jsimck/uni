// Status bar network activity actions, only for IOS
export const ACTIVATE_NETWORK_ACTIVITY = 'ACTIVATE_NETWORK_ACTIVITY';
export const DEACTIVATE_NETWORK_ACTIVITY = 'DEACTIVATE_NETWORK_ACTIVITY';

export function activateNetworkActivity() {
    return {
        type: ACTIVATE_NETWORK_ACTIVITY
    };
}

export function deactivateNetworkActivity() {
    return {
        type: DEACTIVATE_NETWORK_ACTIVITY
    };
}