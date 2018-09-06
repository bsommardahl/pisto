module DiscountFormParams = {
  type state = {
    name: string,
    percent: string,
  };
  type fields = [ | `name | `percent | `price | `taxCalculation | `tags];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (`percent, s => s.percent, (s, percent) => {...s, percent}),
  ];
};

let validationMessage = message =>
  switch (message) {
  | None => ReasonReact.null
  | Some(msg) => <span className="invalid"> {ReactUtils.sloc(msg)} </span>
  };

module EditDiscountForm = ReForm.Create(DiscountFormParams);

let component = ReasonReact.statelessComponent("DiscountEdit");

let make =
    (
      ~discount: option(Discount.t)=None,
      ~onCancel=() => (),
      ~onSubmit,
      ~discounts,
      _children,
    ) => {
  ...component,
  render: _self => {
    let hasDuplicateName = name => {
      let duplicates =
        discounts |> List.filter((c: Discount.t) => c.name === name);
      let isDuplicate = duplicates |> List.length > 0;
      isDuplicate ? Some("validation.duplicate") : None;
    };
    let isUnique = (original: option(Discount.t), new_) =>
      switch (original) {
      | None => hasDuplicateName(new_)
      | Some(disc) =>
        if (disc.name === new_) {
          None;
        } else {
          hasDuplicateName(new_);
        }
      };
    <EditDiscountForm
      onSubmit
      initialState={
        switch (discount) {
        | None => {name: "", percent: ""}
        | Some(disc) => {
            name: disc.name,
            percent: disc.percent |> Percent.toDisplay,
          }
        }
      }
      schema=[
        (`name, Custom(v => v.name |> isUnique(discount))),
        (`percent, Required),
      ]>
      ...{
           ({handleSubmit, handleChange, form, getErrorForField}) => {
             let field = (label, value, fieldType: DiscountFormParams.fields) =>
               <div className="field-input">
                 <label>
                   {ReactUtils.sloc(label)}
                   <input
                     value
                     onChange={
                       ReForm.Helpers.handleDomFormChange(
                         handleChange(fieldType),
                       )
                     }
                   />
                 </label>
                 {validationMessage(getErrorForField(fieldType))}
               </div>;
             <form
               onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
               {field("discount.name", form.values.name, `name)}
               {field("discount.percent", form.values.percent, `percent)}
               <div className="modal-footer">
                 <Button
                   onClick={_ => onCancel()}
                   className="cancel-button-card"
                   label="action.cancelModal"
                   local=true
                 />
                 <Button
                   type_="submit"
                   className="pay-button-card"
                   label="action.done"
                   local=true
                 />
               </div>
             </form>;
           }
         }
    </EditDiscountForm>;
  },
};