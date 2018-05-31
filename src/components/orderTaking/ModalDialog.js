import React,{PropTypes,Component} from 'react';
import ReactDom from 'react-dom';
import {ModalContainer,ModalDialog} from 'react-modal-dialog';

const Modal = props => { 

    let show = props.hasOwnProperty("show");
    
        if(show === true){
            return(
                <div>
                    <ModalContainer onClose={this.closeModal}>
                        <ModalDialog onClose={this.closeModal}>
                            <h1> Are you sure you want to delete this order?</h1>
                            <button className="yesButton"
                             onClick={()=>alert('click')}>
                            Yes
                            </button>
                            <button className="noButton" 
                            onClick={()=>alert('click')}>
                            No
                            </button>
                        </ModalDialog>
                    </ModalContainer>
                </div>
            );
        }     
        else {
            show = false;
            return show;
        }   
}
export default Modal;
