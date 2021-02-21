import React, { Component } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Input, Button } from 'react-bootstrap'
import { pushNotification } from '../../../actions/notifications'
import { ButtonLoading } from '../../../components'
import { loginUser } from '../actions/loginActions'
import { loginSelector } from '../selectors/loginSelector'

class LoginForm extends Component
{
    constructor(props) {
        super(props);

        this.state = {
            login: '',
            pass: '',
            loginInvalid: false,
            passInvalid: false,
            remember: true
        };
    }

    validateState() {
        // Make validations
        this.setState({
            loginInvalid: this.state.login.length <= 0,
            passInvalid: this.state.pass.length <= 0
        });

        // Check if state is invalid
        if (this.state.login.length <= 0 || this.state.pass.length <= 0) {
            // Push notification
            this.props.pushNotification('danger', 'Uživatelské jméno i heslo musí být vyplněny');
            return false;
        }

        // State is valid return true
        return true;
    }

    componentWillReceiveProps(nextProps) {
        if (nextProps.logged) {
            document.location = '/events/';
        }
    }

    handleOnReset() {
        this.setState({
            login: '',
            pass: '',
            loginInvalid: false,
            passInvalid: false,
            remember: true
        });
    }

    handleOnSubmit() {
        if (!this.validateState()) {
            return;
        }

        // Login user
        this.props.loginUser(this.state.login, this.state.pass, this.state.remember);
    }

    linkState(key, value) {
        this.setState({
            [key]: value
        });
    }

    render() {
        return (
            <div className="m-t-20">
                <Input
                    label="Uživatelské jméno:"
                    bsStyle={this.state.loginInvalid ? 'error' : null}
                    onChange={(event) => this.linkState('login', event.target.value)}
                    value={this.state.login}
                    type="text"
                />
                <Input
                    label="Heslo:"
                    bsStyle={this.state.passInvalid ? 'error' : null}
                    onChange={(event) => this.linkState('pass', event.target.value)}
                    value={this.state.pass}
                    type="password"
                />
                <div className="checkbox check-primary m-t-15">
                    <input
                        id="edit-grid-enable"
                        onChange={() => this.linkState('remember', !this.state.remember)}
                        checked={this.state.remember}
                        type="checkbox"
                    />
                    <label htmlFor="edit-grid-enable">
                        Zapamatovat přihlášení
                    </label>
                </div>

                <hr style={{ marginLeft: -20, marginRight: -20, borderColor: '#ebebeb' }} />
                <div className="text-right">
                    <ButtonLoading
                        loading={this.props.processing}
                        disabled={this.props.processing}
                        onClick={() => this.handleOnSubmit()}
                        className="m-r-10"
                        bsStyle="primary"
                    >
                        Přihlásit
                    </ButtonLoading>
                    <Button onClick={() => this.handleOnReset()}>Smazat</Button>
                </div>
            </div>
        );
    }
}

LoginForm.propTypes = {
    processing: React.PropTypes.bool,
    logged: React.PropTypes.bool,

    // Functions
    loginUser: React.PropTypes.func,
    pushNotification: React.PropTypes.func
};

export default connect(
    loginSelector,
    (dispatch) => {
        return bindActionCreators({ pushNotification, loginUser }, dispatch)
    }
)(LoginForm)