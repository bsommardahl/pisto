module WithItemManagement = ItemManagement.Create(DiscountStore);

let component = ReasonReact.statelessComponent("DiscountManagement");

let newItemFromValues =
    (values: DiscountEdit.EditDiscountForm.values)
    : DiscountStore.newItem => {
  name: values.name,
  percent: values.percent |> Percent.toT,
};

let itemFromValues =
    (
      discount: DiscountStore.item,
      values: DiscountEdit.EditDiscountForm.values,
    )
    : DiscountStore.item => {
  id: discount.id,
  name: values.name,
  percent: values.percent |> Percent.toT,
};

let renderItem =
    (~item as discount: DiscountStore.item, ~onEditClick, ~onDeleteClick) =>
  <tr key=discount.id>
    <td>
      <Button
        local=true
        disabled=false
        onClick=onEditClick
        label="action.edit"
      />
    </td>
    <td> (ReactUtils.s(discount.name)) </td>
    <td> (ReactUtils.s(discount.percent |> Percent.toDisplay)) </td>
    <td>
      <Button
        local=true
        className="danger-card"
        onClick=onDeleteClick
        label="action.delete"
      />
    </td>
  </tr>;

let renderCreate = (~items as discounts, ~onSubmit, ~onCancel) =>
  <DiscountEdit
    discounts
    onSubmit=(({values}) => onSubmit(newItemFromValues(values)))
    onCancel
  />;

let renderEdit =
    (~items as discounts, ~item as discount, ~onSubmit, ~onCancel) =>
  <DiscountEdit
    discount=(Some(discount))
    discounts
    onSubmit=(({values}) => onSubmit(itemFromValues(discount, values)))
    onCancel
  />;

let make = _children => {
  ...component,
  render: _self => <WithItemManagement renderCreate renderEdit renderItem />,
};