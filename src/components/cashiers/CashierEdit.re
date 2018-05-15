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

module EditCashierForm = ReForm.Create(CashierFormParams);

let component = ReasonReact.statelessComponent("CashierEdit");

let make = (~name="", ~pin="", ~onSubmit, _children) => {
  ...component,
  render: _self =>
    <EditCashierForm
      onSubmit
      initialState={name, pin}
      schema=[(`name, Required), (`pin, Required)]>
      ...(
           ({handleSubmit, handleChange, form, getErrorForField}) =>
             <form
               onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
               <label>
                 (ReactUtils.s("Name:"))
                 <input
                   value=form.values.name
                   onChange=(
                     ReForm.Helpers.handleDomFormChange(handleChange(`name))
                   )
                 />
               </label>
               <p>
                 (
                   getErrorForField(`name)
                   |> Belt.Option.getWithDefault(_, "")
                   |> ReasonReact.stringToElement
                 )
               </p>
               <label>
                 (ReactUtils.s("Pin:"))
                 <input
                   value=form.values.pin
                   onChange=(
                     ReForm.Helpers.handleDomFormChange(handleChange(`pin))
                   )
                 />
               </label>
               <p>
                 (
                   getErrorForField(`pin)
                   |> Belt.Option.getWithDefault(_, "")
                   |> ReasonReact.stringToElement
                 )
               </p>
               <button _type="submit">
                 ("Submit" |> ReasonReact.stringToElement)
               </button>
             </form>
         )
    </EditCashierForm>,
};