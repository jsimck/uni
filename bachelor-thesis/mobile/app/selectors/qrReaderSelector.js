import { createStructuredSelector } from 'reselect'

export const qrReaderSelector = createStructuredSelector({
    visible: state => state.qrReader.visible,
    qrRead: state => state.qrReader.qrRead,
    events: state => state.events.events,
    games: state => state.games.games
});