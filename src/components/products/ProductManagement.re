module ProductManager = ItemManager.Create(ProductStore);

let component = ReasonReact.statelessComponent("ProductManagement");

let mapValuesToNewProduct =
    (values: ProductEdit.EditProductForm.values): Product.NewProduct.t => {
  sku: values.sku,
  name: values.name,
  suggestedPrice: values.price |> Money.toT,
  taxCalculation:
    values.taxCalculationMethod
    ++ "|"
    ++ values.taxRate
    |> Tax.Calculation.toMethod,
  tags: values.tags |> Tags.toList,
  startDate: None,
  endDate: None,
  department: "",
  unit: "",
  products: [],
  weight: 0,
  location: "",
};

let mapValuesToProduct =
    (product: Product.t, values: ProductEdit.EditProductForm.values)
    : Product.t => {
  id: product.id,
  name: values.name,
  sku: values.sku,
  suggestedPrice: values.price |> Money.toT,
  taxCalculation:
    values.taxCalculationMethod
    ++ "|"
    ++ values.taxRate
    |> Tax.Calculation.toMethod,
  tags: values.tags |> Tags.toList,
  startDate: product.startDate,
  endDate: product.endDate,
  department: product.department,
  weight: product.weight,
  unit: product.unit,
  products: product.products,
  location: product.location,
};

let renderColumns: array(ProductManager.columnRenderer) = [|
  {nameKey: "product.sku", render: product => ReactUtils.s(product.sku)},
  {nameKey: "product.name", render: product => ReactUtils.s(product.name)},
  {
    nameKey: "product.price",
    render: product =>
      ReactUtils.s(product.suggestedPrice |> Money.toDisplay),
  },
  {
    nameKey: "product.taxCalculationMethod",
    render: product =>
      ReactUtils.s(
        product.taxCalculation |> Tax.Calculation.toDelimitedString,
      ),
  },
  {
    nameKey: "product.tags",
    render: product => ReactUtils.s(product.tags |> Tags.toCSV),
  },
|];

let renderCreate = (~items as products, ~onSubmit, ~onCancel) =>
  <ProductEdit
    products
    onSubmit={({values}) => onSubmit(mapValuesToNewProduct(values))}
    onCancel
  />;

let renderEdit = (~items as products, ~item as product, ~onSubmit, ~onCancel) =>
  <ProductEdit
    product={Some(product)}
    products
    onSubmit={({values}) => onSubmit(mapValuesToProduct(product, values))}
    onCancel
  />;

let make = _children => {
  ...component,
  render: _self =>
    <ProductManager
      name="product"
      headerKey="admin.products.header"
      renderCreate
      renderEdit
      renderColumns
    />,
};