module CashierFormParams = {
  type state = {
    name: string,
    pin: string,
  };
  type fields = [ | `name | `pin];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (`pin, s => s.pin, (s, pin) => {...s, pin}),
  ];
};

let validationMessage = message =>
  switch (message) {
  | None => ReasonReact.null
  | Some(msg) => <span className="invalid"> (ReactUtils.sloc(msg)) </span>
  };

module EditCashierForm = ReForm.Create(CashierFormParams);

let component = ReasonReact.statelessComponent("CashierEdit");

let make =
    (~name="", ~pin="", ~onSubmit, ~isUnique, ~onCancel=() => (), _children) => {
  ...component,
  render: _self =>
    <EditCashierForm
      onSubmit
      initialState={name, pin}
      schema=[
        (`name, Required),
        (`pin, Custom(v => v.pin |> isUnique(pin))),
      ]>
      ...(
           ({handleSubmit, handleChange, form, getErrorForField}) =>
             <form
               onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
               <div>
                 (ReactUtils.sloc("cashier.name"))
                 <input
                   value=form.values.name
                   onChange=(
                     ReForm.Helpers.handleDomFormChange(handleChange(`name))
                   )
                 />
                 (validationMessage(getErrorForField(`name)))
               </div>
               <div>
                 (ReactUtils.sloc("cashier.pin"))
                 <input
                   value=form.values.pin
                   onChange=(
                     ReForm.Helpers.handleDomFormChange(handleChange(`pin))
                   )
                 />
                 (validationMessage(getErrorForField(`pin)))
               </div>
               <div className="modal-footer">
                 <Button
                   onClick=(_ => onCancel())
                   className="cancel-button-card"
                   label="action.cancelModal"
                   local=true
                 />
                 <Button
                   _type="submit"
                   className="pay-button-card"
                   label="action.done"
                   local=true
                 />
               </div>
             </form>
         )
    </EditCashierForm>,
};